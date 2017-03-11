function y=filtro_orden2(b1i,a1i,a2i,x)
v=[b1i, a1i, a2i];

I=ceil(log2(max(abs(v))));
Q=16-1-I;

a1=round(a1i*2^Q); %<16,12>
a2=round(a2i*2^Q); %<16,12>
b1=round(b1i*2^Q); %<16,12>


y=zeros(1,48000);  
acum_1=0; 
acum_2=0;  
 
for i= 1:length(x)
    x_0=x(i); %<16, 15>
    x_1=x_0*b1; %<16 15>*<16,Q> = 15+Q
    x_2=x_0*2^Q; %<16 15> <<Q = 15+Q
  
    y(i)=(x_0+(acum_1)*2^(-Q)); %<32,15+Q>+<32,15+Q> >>Q=<16,15>
    y_1=y(i)*(-a1); %<16,15>*<16,Q> = <32,15+Q>
    y_2=y(i)*(-a2); %<16,15>*<16,Q> = <32,15+Q>
        
    acum_1=x_1+y_1+acum_2;
    acum_2=x_2+y_2; 
end

end
