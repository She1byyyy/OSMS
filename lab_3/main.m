clear; 

f1 = 4;
f2 = 8;
f3 = 9;

t = 0:0.01:1;

s1 = cos(2*pi*f1*t);
s2 = cos(2*pi*f2*t);
s3 = cos(2*pi*f3*t);

a = 3*s1 + 3*s2 + s3;
b = s1 + 0.5 * s2;

figure;
subplot(2,1,1);
plot(t, a);
title('Сигнал a(t)');
subplot(2,1,2);
plot(t, b);
title('Сигнал b(t)');

corr_ab = sum(a .* b);
norm_corr_ab = sum(a .* b) / (sqrt(sum(a.^2)) * sqrt(sum(b.^2)));

disp(['Корреляция между сигналами a(t) и b(t): ', num2str(corr_ab)]);
disp(['Нормализованная корреляция между сигналами a(t) и b(t): ', num2str(norm_corr_ab)]);

t = 0:0.01:1;

a = [0.3 0.2 -0.1 4.2 -2 1.5 0];
b = [0.3 4 -2.2 1.6 0.1 0.1 0.2];

figure;
subplot(2,1,1);
stem(a, '-o');
title('Array a');

subplot(2,1,2);
stem(b, '-o');
title('Array b');

% Вычисляем взаимную корреляцию
corr_ab = xcorr(a, b);

N = length(a);

norm_factor_ab = sqrt(sum(a.^2) * sum(b.^2));

norm_corr_ab = corr_ab / norm_factor_ab;

%norm_corr_ab = (norm_corr_ab - min(norm_corr_ab)) / (max(norm_corr_ab) - min(norm_corr_ab)) * 2 - 1;

figure;
plot(norm_corr_ab);
title('Взаимная корреляция a и b');

n = length(a);
corr_vals_ab = zeros(1, n);

for i = 0:n-1
    b_shifted = circshift(b, i);
    
    corr_vals_ab(i+1) = sum(a .* b_shifted);
end

% Визуализация корреляции для a и b
figure;
plot(0:n-1, corr_vals_ab, '-o');
title('Зависимость корреляции a и b от величины сдвига');
xlabel('Сдвиг');
ylabel('Корреляция');

[max_corr_ab, max_shift_ab] = max(corr_vals_ab);
disp(['Максимальная корреляция между a и b: ', num2str(max_corr_ab)]);
disp(['Сдвиг, при котором достигается максимальная корреляция для a и b: ', num2str(max_shift_ab-1)]);

figure;
subplot(2,1,1);
plot(a, '-o');
title('Сигнал a');

subplot(2,1,2);
plot(norm_corr_ab, '-o');
title('Сигнал b, сдвинутый на величину максимальной корреляции с a');
