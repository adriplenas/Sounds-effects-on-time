%% Efecto telefonico 
clear all; close all;

Fm = 48000;
Wp = [300 3200]/(Fm/2);
Ws = [200 3600]/(Fm/2);

Rp = 1;
Rs = 80;
[b,a] = ellip(6,Rp,Rs,Wp);%Orden 2 para el filtro es sufuciente para lo requerido
[sos,g] = tf2sos(b,a);%para simplificar el proceso separaremos el filtro 

%cuantizar la ganancia del filtro
Ig = ceil(log2(g));
Qg = 16-1-Ig;
G = round(g*2^Qg);
gp = g^(1/6);
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

%filtro3
b3 = sos(3,1:3);
a3 = sos(3,4:end);

Ib3 = ceil(log2(max(abs(b3))));
Qb3 = 16-1-Ib3;
Ia3 = ceil(log2(max(abs(a3))));
Qa3 = 16-1-Ia3;

b3f = round(b3*2^Qb3);
a3f = round(a3*2^Qb3);
%filtro4
b4 = sos(4,1:3);
a4 = sos(4,4:end);

Ib4 = ceil(log2(max(abs(b4))));
Qb4 = 16-1-Ib4;
Ia4 = ceil(log2(max(abs(a4))));
Qa4 = 16-1-Ia4;

b4f = round(b4*2^Qb4);
a4f = round(a4*2^Qb4);

%filtro5
b5 = sos(5,1:3);
a5 = sos(5,4:end);

Ib5 = ceil(log2(max(abs(b5))));
Qb5 = 16-1-Ib5;
Ia5 = ceil(log2(max(abs(a5))));
Qa5 = 16-1-Ia5;

b5f = round(b5*2^Qb5);
a5f = round(a5*2^Qb5);


%filtro6
b6 = sos(6,1:3);
a6 = sos(6,4:end);

Ib6 = ceil(log2(max(abs(b6))));
Qb6 = 16-1-Ib6;
Ia6 = ceil(log2(max(abs(a6))));
Qa6 = 16-1-Ia6;

b6f = round(b6*2^Qb6);
a6f = round(a6*2^Qb6);

%representacion
Fs = 48000;
N = 100000;
[H,F] = freqz(b,a,N,Fs);
[H1,F] = freqz(b1,a1,N,Fs);
[H2,F] = freqz(b2,a2,N,Fs);
[H3,F] = freqz(b3,a3,N,Fs);
[H4,F] = freqz(b4,a4,N,Fs);
[H5,F] = freqz(b5,a5,N,Fs);
[H6,F] = freqz(b6,a6,N,Fs);
figure(1);
plot(F,10*log10(abs(H))); hold on;
figure(2);
subplot(321); plot(F,10*log10(abs(H1))); hold on;
subplot(322); plot(F,10*log10(abs(H2))); hold on;
subplot(323); plot(F,10*log10(abs(H3))); hold on;
subplot(324); plot(F,10*log10(abs(H4))); hold on;
subplot(325); plot(F,10*log10(abs(H5))); hold on;
subplot(326); plot(F,10*log10(abs(H6))); hold on;

x = [1 zeros(1,47999)];
x = round(x*2^15);%Cuantizar la entrada al valor máximo

%respuesta impulsional de los filtros por separado
salida_1 = filtro_orden2(b1(2),a1(2),a1(3),x);%Respuesta impulsional del primer filtro
salida_2 = filtro_orden2(b2(2),a2(2),a2(3),x);%Respuesta impulsional del segundo filtro
salida_3 = filtro_orden2(b3(2),a3(2),a3(3),x);%Respuesta impulsional del tercer filtro
salida_4 = filtro_orden2(b4(2),a4(2),a4(3),x);%Respuesta impulsional del cuarto filtro
salida_5 = filtro_orden2(b5(2),a5(2),a5(3),x);%Respuesta impulsional del quinto filtro
salida_6 = filtro_orden2(b6(2),a6(2),a6(3),x);%Respuesta impulsional del sexto filtro

%Respuesta impulsional del filtro en cascada resultante
h1=filtro_orden2(b1(2),a1(2),a1(3),x);
h2=filtro_orden2(b2(2),a2(2),a2(3),h1);
h3=filtro_orden2(b3(2),a3(2),a3(3),h2);
h4=filtro_orden2(b4(2),a4(2),a4(3),h3);
h5=filtro_orden2(b5(2),a5(2),a5(3),h4);
salida_filtro=filtro_orden2(b6(2),a6(2),a6(3),h5);
salida_filtro = salida_filtro*2^(-15)*G*2^(-Qg);%Ganancia del filtro en cascada ajustada

%respuesta impulsional del filtro implementado

h1=filtro_orden2(b1(2),a1(2),a1(3),x);
h2=filtro_orden2(b2(2),a2(2),a2(3),h1*Gp*2^(-Qgp));
h3=filtro_orden2(b3(2),a3(2),a3(3),h2*Gp*2^(-Qgp));
h4=filtro_orden2(b4(2),a4(2),a4(3),h3*Gp*2^(-Qgp));
h5=filtro_orden2(b5(2),a5(2),a5(3),h4*Gp*2^(-Qgp));
filtro_implementado=filtro_orden2(b6(2),a6(2),a6(3),h5*Gp*2^(-Qgp));
filtro_implementado = filtro_implementado*2^(-15);

salida_1 = salida_1*2^(-15);%Devolver las salidas a la escala inicial <Q,0>
salida_2 = salida_2*2^(-15);%Devolver las salidas a la escala inicial <Q,0>
salida_3 = salida_3*2^(-15);%Devolver las salidas a la escala inicial <Q,0>
salida_4 = salida_4*2^(-15);%Devolver las salidas a la escala inicial <Q,0>
salida_5 = salida_5*2^(-15);%Devolver las salidas a la escala inicial <Q,0>
salida_6 = salida_6*2^(-15);%Devolver las salidas a la escala inicial <Q,0>

FH = fftshift(fft(salida_filtro));
FHI = fftshift(fft(filtro_implementado));
FH1 = fftshift(fft(salida_1));
FH2 = fftshift(fft(salida_2));
FH3 = fftshift(fft(salida_3));
FH4 = fftshift(fft(salida_4));
FH5 = fftshift(fft(salida_5));
FH6 = fftshift(fft(salida_6));

figure(1);
plot(10*log10(abs(FH(24001:48000))),'r'); hold on;
plot(10*log10(abs(FHI(24001:48000))),'g');
title('respuesta global'); 
legend('Filtro original','Filtro en C cuantificado','Filtro implementado'); ylabel('dB'); xlabel('f(kHz)');
figure(2);
subplot(321); plot(10*log10(abs(FH1(24001:48000))),'r'); title('primer filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');
subplot(322); plot(10*log10(abs(FH2(24001:48000))),'r'); title('segundo filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');
subplot(323); plot(10*log10(abs(FH3(24001:48000))),'r'); title('tercer filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');
subplot(324); plot(10*log10(abs(FH4(24001:48000))),'r'); title('cuarto filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');
subplot(325); plot(10*log10(abs(FH5(24001:48000))),'r'); title('quinto filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');
subplot(326); plot(10*log10(abs(FH6(24001:48000))),'r'); title('sexto filtro'); 
legend('Filtro original','Filtro en C cuantificado'); ylabel('dB'); xlabel('f(kHz)');

%valores de B
maximo_1 = sum(salida_1); B1 = log2(maximo_1)
maximo_2 = sum(salida_2); B2 = log2(maximo_2)
maximo_3 = sum(salida_3); B3 = log2(maximo_3)
maximo_4 = sum(salida_4); B4 = log2(maximo_4)
maximo_5 = sum(salida_5); B5 = log2(maximo_5)
maximo_6 = sum(salida_6); B6 = log2(maximo_6)


%% Prueba del efecto robot con Female
load('female.mat');

%%filtrar la señal

h1=filtro_orden2(b1(2),a1(2),a1(3),female);
h2=filtro_orden2(b2(2),a2(2),a2(3),h1*Gp*2^(-Qgp));
h3=filtro_orden2(b3(2),a3(2),a3(3),h2*Gp*2^(-Qgp));
h4=filtro_orden2(b4(2),a4(2),a4(3),h3*Gp*2^(-Qgp));
h5=filtro_orden2(b5(2),a5(2),a5(3),h4*Gp*2^(-Qgp));
female_filtrada=filtro_orden2(b6(2),a6(2),a6(3),h5*Gp*2^(-Qgp));
female_filtrada = female_filtrada*Gp*2^(-Qgp);

soundsc(female,Fs);
soundsc(female_filtrada,Fs);

%%visualizacion de la señal filtrada frente a la original
Ffemale = fft(female);
Ffemale_filtrada = fft(female_filtrada);

figure;
F = linspace(-Fs/2,Fs/2,length(Ffemale));
plot(F,fftshift(abs(Ffemale))); hold on;
plot(F,fftshift(abs(Ffemale_filtrada)),'r');
xlim([-1e4 1e4]);xlabel('f(kHz)');
legend('female','female filtrada');
%% pruebas con Bohemian Rhapsody
[queen,FS,NBITS]=wavread('Queen.wav');
queen=queen(1:end,1)+queen(1:end,2);%guardar los dos canales en una variable%%filtrar la señal

%filtrar la señal
h1=filtro_orden2(b1(2),a1(2),a1(3),queen);
h2=filtro_orden2(b2(2),a2(2),a2(3),h1*Gp*2^(-Qgp));
h3=filtro_orden2(b3(2),a3(2),a3(3),h2*Gp*2^(-Qgp));
h4=filtro_orden2(b4(2),a4(2),a4(3),h3*Gp*2^(-Qgp));
h5=filtro_orden2(b5(2),a5(2),a5(3),h4*Gp*2^(-Qgp));
queen_filtrada=filtro_orden2(b6(2),a6(2),a6(3),h5*Gp*2^(-Qgp));
queen_filtrada = queen_filtrada*Gp*2^(-Qgp);

soundsc(queen,FS);
soundsc(queen_filtrada,FS);

%%visualizacion de la señal filtrada frente a la original
Fqueen = fft(queen);
Fqueen_filtrada = fft(queen_filtrada);

figure;
F = linspace(-Fs/2,Fs/2,length(Fqueen));
plot(F,fftshift(abs(Fqueen))); hold on;
plot(F,fftshift(abs(Fqueen_filtrada)),'r');
xlim([-1e4 1e4]);
%% grabar en wav
wavwrite(female,Fs,'Female.wav');
wavwrite(female_filtrada,Fs,'Female_filtrada.wav');
wavwrite(queen_filtrada,FS,'Queen_filtrada.wav');