%% Efecto robot
clear all; close all;
%Filtro paso banda necesario para eliminar frecuencias no audibles y
%frecuencias bajas para eliminar algo de aliasing

Fm = 48000;
Wp = [1000 4000]/Fm;
Ws = [800 4400]/Fm;

Rp = 1;
Rs = 80;
[b,a] = ellip(2,Rp,Rs,Wp);%Orden 2 para el filtro es sufuciente para lo requerido
[sos,g] = tf2sos(b,a);%para simplificar el proceso separaremos el filtro 

%cuantizar la ganancia del filtro
Ig = ceil(log2(g));
Qg = 16-1-Ig;
G = round(g*2^Qg);
gp = sqrt(g);
Igp = ceil(log2(gp));
Qgp = 16-1-Igp;
Gp = round(gp*2^Qgp);

%filtro1
b1 = sos(1,1:3);
a1 = sos(1,4:end);
Ib1 = ceil(log2(max(abs(b1))));
Qb1 = 16-1-Ib1;
Ia1 = ceil(log2(max(abs(a1))));
Qa1 = 16-1-Ia1;

b1f = round(b1*2^Qb1);
a1f = round(a1*2^Qb1);

%filtro2
b2 = sos(2,1:3);
a2 = sos(2,4:end);

Ib2 = ceil(log2(max(abs(b2))));
Qb2 = 16-1-Ib2;
Ia2 = ceil(log2(max(abs(a2))));
Qa2 = 16-1-Ia2;

b2f = round(b2*2^Qb2);
a2f = round(a2*2^Qb2);

%representacion
Fs = 48000;
N = 100000;
[H,F] = freqz(b,a,N,Fs);
[H1,F] = freqz(b1,a1,N,Fs);
[H2,F] = freqz(b2,a2,N,Fs);
figure(1);
plot(F,10*log10(abs(H))); hold on;
figure(2);
subplot(211); plot(F,10*log10(abs(H1))); hold on;
subplot(212); plot(F,10*log10(abs(H2))); hold on;

x = [1 zeros(1,47999)];
x = round(x*2^15);%Cuantizar la entrada al valor máximo

salida_1 = filtro_orden2(b1(2),a1(2),a1(3),x);%Respuesta impulsional del primer filtro
salida_2 = filtro_orden2(b2(2),a2(2),a2(3),x);%Respuesta impulsional del segundo filtro
salida_filtro = filtro_orden2(b2(2),a2(2),a2(3),salida_1);%Respuesta impulsional del filtro en cascada resultante
salida_filtro = salida_filtro*2^(-15)*G*2^(-Qg);%Ganancia del filtro en cascada ajustada
filtro_implementado = filtro_orden2(b2(2),a2(2),a2(3),salida_1*Gp*2^(-Qgp));
filtro_implementado = filtro_implementado*2^(-15);

salida_1 = salida_1*2^(-15);%Devolver las salidas a la escala inicial <Q,0>
salida_2 = salida_2*2^(-15);%Devolver las salidas a la escala inicial <Q,0>

FH = fftshift(fft(salida_filtro));
FHI = fftshift(fft(filtro_implementado));
FH1 = fftshift(fft(salida_1));
FH2 = fftshift(fft(salida_2));

figure(1);
plot(10*log10(abs(FH(24001:48000))),'r'); hold on;
plot(10*log10(abs(FHI(24001:48000))),'g');
title('respuesta global'); 
legend('Filtro original','Filtro en C cuantificado','Filtro implementado'); ylabel('dB'); xlabel('f(kHz)');
figure(2);
subplot(211); plot(10*log10(abs(FH1(24001:48000))),'r'); title('primer filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');
subplot(212); plot(10*log10(abs(FH2(24001:48000))),'r'); title('segundo filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');

%bits extra necesarios para la respuesta del filtro
maximo_1 = sum(abs(salida_1)); B1 = ceil(log2(maximo_1))
maximo_2 = sum(abs(salida_2)); B2 = ceil(log2(maximo_2))

%% Prueba del efecto robot con Female
load('female.mat');
Fs = 48000;
N = 100000;

n = linspace(0,length(female)/Fs,length(female));
cos_desp = cos(2*pi*n*200);
female_desp = female.*cos_desp';

%%filtrar la señal
salida_1 = filtro_orden2(b1(2),a1(2),a1(3),female_desp);
female_filtrada = filtro_orden2(b2(2),a2(2),a2(3),salida_1);
female_filtrada = female_filtrada*G*2^(-Qg);
female_implementado = filtro_orden2(b2(2),a2(2),a2(3),salida_1*Gp*2^(-Qgp));

%reproducir la señal
soundsc(female,Fs);
soundsc(female_implementado,Fs);
soundsc(female_filtrada,Fs);

%%visualizacion de la señal filtrada frente a la original
Ffemale = fft(female);
Ffemale_desp = fft(female_desp);
Ffemale_filtrada = fft(female_filtrada);
figure;
F = linspace(-Fs/2,Fs/2,length(Ffemale));
plot(F,fftshift(abs(Ffemale))); hold on;
plot(F,fftshift(abs(Ffemale_desp)),'r'); hold on;
plot(F,fftshift(abs(Ffemale_filtrada)),'g'); hold on;
xlim([-1e4 1e4]); xlabel('f(kHz)');
legend('female','female desplazada','female filtrada');
%% Pruebas con Bohemian Rhapsody
[queen,FS,NBITS]=wavread('Queen.wav');
queen=queen(1:end,1)+queen(1:end,2);%guardar los dos canales en una variable
Fs = 48000;
N = 100000;
n = linspace(0,length(queen)/Fs,length(queen));
cos_desp = cos(2*pi*n*200);
queen_desp = queen.*cos_desp';

%%filtrar la señal
salida_1 = filtro_orden2(b1(2),a1(2),a1(3),queen_desp);
queen_filtrada = filtro_orden2(b2(2),a2(2),a2(3),salida_1);
queen_filtrada = queen_filtrada*G*2^(-Qg);
queen_implementado = filtro_orden2(b2(2),a2(2),a2(3),salida_1*Gp*2^(-Qgp));

%reproducir la señal
soundsc(queen,FS);
soundsc(queen_implementado,FS);
soundsc(queen_filtrada,FS);

%%visualizacion de la señal filtrada frente a la original
Fqueen = fft(queen);
Fqueen_desp = fft(queen_desp);
Fqueen_filtrada = fft(queen_filtrada);
figure;
F = linspace(-Fs/2,Fs/2,length(Fqueen));
plot(F,fftshift(abs(Fqueen))); hold on;
plot(F,fftshift(abs(Fqueen_desp)),'r'); hold on;
plot(F,fftshift(abs(Fqueen_filtrada)),'g'); hold on;
xlim([-1e4 1e4]); xlabel('f(kHz)');
legend('Queen','Queen desplazada','Queen filtrada');

%% guardar la señal en .wav
wavwrite(female,Fs,'Female.wav');
wavwrite(female_filtrada,Fs,'Female_filtrada.wav');
wavwrite(queen_filtrada,FS,'Queen_filtrada.wav');