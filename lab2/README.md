# Practice_2

## Цель:
Ознакомиться с основами проектирования покрытия сетей мобильной связи и научиться рассчитывать радиус действия базовых станций, что необходимо для создания качественной системы радиосвязи.

# Радиобюджет (Uplink, Downlink) и Модели Распространения
Проект охватывает ключевые аспекты радиобюджета, включая **uplink** и **downlink** бюджеты, и рассматривает различные модели распространения радиоволн для оценки потерь сигнала. В частности, приводятся модели **Uminlos**, **Cost 231 Hata** и **Walfish-Ikegami**.

## Содержание
1. [Введение](#введение)
2. [Uplink и Downlink бюджеты](#uplink-и-downlink-бюджеты)
3. [Uminlos](#uminlos)
4. [Cost 231 Hata](#cost-231-hata)
5. [Walfish-Ikegami](#walfish-ikegami)
6. [Заключение](#заключение)

---

## Введение
При разработке радиосистем важно учитывать потери на пути распространения радиоволн для обеспечения качественной связи. Основой процесса является расчет бюджетов **uplink** и **downlink** (восходящей и нисходящей линии связи), а также использование подходящих моделей потерь в различных условиях городской и пригородной среды.

---

## Uplink и Downlink бюджеты

### Uplink Budget (Бюджет восходящей линии связи)
**Uplink** бюджет описывает передачу сигнала от устройства к базовой станции и включает:
- Мощность передачи устройства.
- Потери на пути распространения.
- Усиление антенн.
- Влияние шума и помех.

### Downlink Budget (Бюджет нисходящей линии связи)
**Downlink** бюджет отражает передачу сигнала от базовой станции к устройству и учитывает:
- Мощность передачи базовой станции.
- Потери на пути.
- Усиление антенн на стороне станции и устройства.
- Уровень шума на приемнике.

### Основное уравнение для бюджетов:
\[
{Мощность на приемнике} = {Мощность передатчика} + {Усиление антенн} - {Потери на пути}
\]

При расчетах важно учитывать потери на пути распространения, определяемые с помощью моделей, таких как **Uminlos**, **Cost 231 Hata** и **Walfish-Ikegami**.

---

## Uminlos
**Uminlos** (Urban Micro Non-Line-of-Sight) — модель для расчета потерь в условиях плотной городской застройки при отсутствии прямой видимости. Она учитывает:
- Плотность и высоту зданий.
- Характеристики строительных материалов.
- Частоту сигнала и расстояние.

Формула для расчета потерь:
\[
L = L_0 + 10 * n * log_{10}(d)
\]
где:
- \(L_0\) — потери в свободном пространстве,
- \(n\) — коэффициент затухания,
- \(d\) — расстояние между передатчиком и приемником.

---

## Cost 231 Hata
**Cost 231 Hata** — это расширение модели Хата, применимое на частотах от 500 МГц до 2 ГГц для городских и пригородных условий. Она учитывает высоту антенн, частоту и тип застройки.

Формула для расчета потерь:
\[
L = 46.3 + 33.9 * log_{10}(f) - 13.82 * log_{10}(h_b) + (44.9 - 6.55 * log_{10}(h_b)) * log_{10}(d) + C
\]
где:
- \(f\) — частота в МГц,
- \(h_b\) — высота антенны,
- \(d\) — расстояние до приемника,
- \(C\) — коэффициент для городской или пригородной среды.

---

## Walfish-Ikegami
**Модель Walfish-Ikegami** используется для расчета потерь в городской среде с высокой плотностью застройки, учитывая рассеивание и дифракцию радиоволн на зданиях.

### Этапы расчета:
1. **Прямое распространение** (LoS) — при отсутствии преград.
2. **Непрямое распространение** (NLoS) — отражения и дифракции на зданиях.

Формула потерь:
\[
L = L_{FS} + L_{rts} + L_{msd}
\]
где:
- \(L_{FS}\) — потери в свободном пространстве,
- \(L_{rts}\) — потери от рассеивания,
- \(L_{msd}\) — потери от дифракции между зданиями.

---

## Заключение
Каждая модель имеет свою специфику и подходит для различных условий. Учет бюджетов **uplink** и **downlink** и правильный выбор модели потерь — основные этапы проектирования радиосистем.

---

**Примечание**: Точные расчеты требуют учета частоты, высоты антенн, расстояния и среды.
