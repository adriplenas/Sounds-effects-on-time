%% Cuantizacion de constantes 
clear all; close all;
% eco y reverb
tiempo_muestreo = 1/480; %100/48k
Itm = ceil(log2(tiempo_muestreo));
Qtm = 16-Itm;
tiempo_muestreo_C = round(tiempo_muestreo*2^Qtm);
error_tm = tiempo_muestreo*abs((1-tiempo_muestreo_C*2^(-Qtm)/tiempo_muestreo))*100 %error en cuantizacion

atenuacion = 0.33;
Ia = ceil(log2(atenuacion));
Qa = 16-Ia;
atenuacion_C = round(atenuacion*2^Qa);
error_a = atenuacion*abs((1-atenuacion_C*2^(-Qa)/atenuacion))*100 %error en cuantizacion

%coro
atenuacion_coro = 0.25;
Iac = ceil(log2(atenuacion_coro));
Qac = 16-Iac;
atenuacion_coro_C = round(atenuacion_coro*2^16);%lo dejamos en escala 16,16
error_a_coro = atenuacion_coro*abs((1-atenuacion_coro_C*2^(-Qac)/atenuacion_coro))*100 %error en cuantizacion

%wahwah
tiempo_wah = 1/48000;
It = ceil(log2(tiempo_wah));
Qt = 16-It;
tiempo_wah_C = round(tiempo_wah*2^Qt);
error_t_wah = tiempo_wah*abs((1-tiempo_wah_C*2^(-Qt)/tiempo_wah))*100 %error en cuantizacion

ganancia_inv = 1/480; %1/480
Igi = ceil(log2(ganancia_inv));
Qgi = 16-Igi;
ganancia_inv_C = round(ganancia_inv*2^Qgi);
error_g_inv = ganancia_inv*abs((1-ganancia_inv_C*2^(-Qgi)/ganancia_inv)) %error en cuantizacion

%robot
fc = 48000;
f0 = 200;
N = 8;
M = round(2^8*f0*2^N/fc);%M la codificamos en <16,8>
f_efect = M/2^8*fc/2^N; %para ver la frecuencia efectiva, tomamos M en <16,0>
error = f0*(1-f_efect/f0) %error en cuantizacion

%errores de cuantizacion minimos
%error en el calculo de la frecuencia más alto pero despreciable 