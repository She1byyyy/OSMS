#include <iostream>
#include <memory>
#include "io.hpp"

#define SIGNAL_CORRELATION_REFERENCE_THRES 0.9
#define DATA_DIR "./data"
#define SIGNAL_CHANNEL_LENGTH 30000
#define SIGNAL_BUFFER_SIZE 20000
#define SIGNAL_OVERSAMPLING 10

using namespace std;
using namespace io;

// объявления функций
size_t read_referenceChannel(vector<double> ref, int repeatSamples);
vector<bool> generateSamples(int repeatSamples, vector<bool> payload);
vector<bool> repeat_elements(const vector<bool>& input, int repeat_count);

vector<bool> repeat_elements(const vector<bool>& input, int repeat_count) {
    vector<bool> result;
    result.reserve(input.size() * repeat_count);  // резервируем память для результата

    for (const bool& element : input) {
        for (int i = 0; i < repeat_count; ++i) {
            result.push_back(element);
        }
    }

    return result;
}

int main(int argc, const char* argv[]) {
    vector<bool> test = {1, 1, 0, 1, 0, 0, 1, 0, 0, 1};

    // ввод имени и фамилии
    string payload;
    cout << "Enter a firstname lastname: ";

    if (!getline(cin, payload)) {
        cerr << "Error reading input" << "\n";
    }

    // ввод значения sigma (отношение сигнал/шум)
    float sigma = 0;
    cout << "Enter a sigma range(0:1): ";

    if (!(cin >> sigma)) {
        cerr << "Error reading input sigma" << "\n";
    }

    if (sigma > 1 || sigma < 0) {
        cerr << "Incorrect sigma value" << "\n";
    }

    cout << "Sigma= " << sigma << endl << "Name= " << payload << endl;

    // кодирование имени и фамилии в ASCII-биты
    vector<bool> ascii_payload = ascii_encode_string(payload);

    // инициализация генератора случайных чисел для моделирования шума
    std::mt19937 generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, sigma);

    // генерация шумового канала
    vector<double> radioChannelNoise(SIGNAL_CHANNEL_LENGTH);
    vector<double> radioChannelReference(SIGNAL_CHANNEL_LENGTH, 0.0);

    for (double& noise : radioChannelNoise) {
        noise = distribution(generator);
    }

    // сохранение шума в файл
    std::string simFrameNoise = "noise.txt";
    std::ofstream fileNoise(simFrameNoise);
    if (!fileNoise.is_open()) {
        cerr << "Error opening noise file" << "\n";
        return EXIT_FAILURE;
    }

    for (const double& val : radioChannelNoise) {
        fileNoise << val << endl;
    }

    fileNoise.close();

    // генерация повторяющихся образцов
    vector<bool> repeated_frame = generateSamples(10, ascii_payload);

    // сохранение образцов в файл
    std::string simFrame1 = "buffer_samples.txt";
    std::ofstream fileFrameSamples(simFrame1);
    if (!fileFrameSamples.is_open()) {
        cerr << "Error opening frame samples file" << "\n";
        return EXIT_FAILURE;
    }

    for (const bool& val : repeated_frame) {
        fileFrameSamples << val;
    }

    fileFrameSamples.close();

    // сохранение шума и образцов в файлы
    std::string simNoiseFrame = "noise_and_frame.txt";
    std::ofstream fileNoiseFrame(simNoiseFrame);
    if (!fileNoiseFrame.is_open()) {
        cerr << "Error opening noise and frame file" << "\n";
        return EXIT_FAILURE;
    }
    std::string simReference = "reference.txt";
    std::ofstream fileReference(simReference);
    if (!fileReference.is_open()) {
        cerr << "Error opening reference file" << "\n";
        return EXIT_FAILURE;
    }

    // добавление образцов в шумовой канал
    int startPos = 100;
    for (bool val : repeated_frame) {
        if (val) {
            radioChannelNoise[startPos] += 1;
        } else {
            radioChannelNoise[startPos] -= 1;
        }
        if (val) {
            radioChannelReference[startPos] += 1;
        } else {
            radioChannelReference[startPos] -= 1;
        }
        startPos++;
    }

    for (const double& val : radioChannelNoise) {
        fileNoiseFrame << val << endl;
    }
    for (const double& val : radioChannelReference) {
        fileReference << val << endl;
    }

    fileNoiseFrame.close();
    fileReference.close();

    std::string simSignal_In = "signal_in.txt";
    std::ofstream fileSignal_In(simSignal_In);
    if (!fileSignal_In.is_open()) {
        cerr << "Error opening signal input file" << "\n";
        return EXIT_FAILURE;
    }

    read_referenceChannel(radioChannelReference, 10);
    return EXIT_SUCCESS;
}

// функция для проверки корреляции сдвигающегося окна
bool window_slide_correlation(const vector<bool>& buff, const vector<bool>& gold_seq, double threshold) {
    if (buff.size() >= gold_seq.size()) {
        vector<bool> recent_bits(buff.end() - gold_seq.size(), buff.end());
        long double corr = io::seq::nbitcorr(recent_bits, gold_seq);

        if (corr > threshold) {
            return true;
        }
    }
    return false;
}

// функция для чтения эталонного канала и поиска корреляции
size_t read_referenceChannel(vector<double> ref, int repeatSamples) {
    vector<bool> buff;
    const uint8_t regXGold = 0b01011;
    const uint8_t regYGold = 0b11011;

    vector<bool> gold_seq = repeat_elements(io::seq::gold_generate(regXGold, regYGold), 10);

    size_t corrIndex = 0;

    for (size_t scan = 0; scan < 500; ++scan) {
        buff.push_back(ref[scan] > 0);

        if (window_slide_correlation(buff, gold_seq, SIGNAL_CORRELATION_REFERENCE_THRES)) {
            corrIndex = scan;
            break;
        }
    }
    corrIndex += 1;

    buff.erase(buff.begin(), buff.begin() + corrIndex);
    vector<bool> tmpBuff;

    for (size_t scan = corrIndex; scan < SIGNAL_CHANNEL_LENGTH; scan += repeatSamples) {
        tmpBuff.push_back(ref[scan] > 0);

        if (io::crc8_check(tmpBuff)) {
            bool hasTenOnes = true;
            for (size_t i = 0; i < 5; ++i) {
                size_t checkIndex = scan + 1 + i;
                if (checkIndex >= SIGNAL_CHANNEL_LENGTH || ref[checkIndex] != 1) {
                    hasTenOnes = false;
                    break;
                }
            }

            if (hasTenOnes) {
                break;
            }
        }
    }

    if (tmpBuff.size() > 7) {
        size_t elementsToCopy = tmpBuff.size() - 7;
        buff.insert(buff.end(), tmpBuff.begin(), tmpBuff.begin() + elementsToCopy);
    }

    std::string receivedString = io::ascii_decode_string(buff);
    std::cout << receivedString << std::endl;
    return corrIndex;
}

// функция для генерации образцов данных с добавлением CRC и последовательности Голда
vector<bool> generateSamples(int repeatSamples, vector<bool> payload) {
    const uint8_t regXGold = 0b01011;
    const uint8_t regYGold = 0b11011;

    // генерация последовательности Голда
    vector<bool> gold_seq = io::seq::gold_generate(regXGold, regYGold);

    vector<bool> frame;
    frame.insert(frame.end(), payload.begin(), payload.end());

    uint8_t crc3 = crc8_calc(frame);

    for (int i = 6; i >= 0; --i) {
        frame.push_back((crc3 >> i) & 1);
    }

    vector<bool> combined;
    combined.insert(combined.end(), gold_seq.begin(), gold_seq.end());
    combined.insert(combined.end(), frame.begin(), frame.end());

    vector<bool> div = {1, 1, 1, 1, 1};
    combined.insert(combined.end(), div.begin(), div.end());

    return repeat_elements(combined, repeatSamples);
}
