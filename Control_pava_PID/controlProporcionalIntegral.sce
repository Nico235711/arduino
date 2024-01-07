tauG    = 4490/60; //74.83;
s       = %s;
A       = 0.9/60;
G       = A/(tauG * s + 1);

Td      = 36/60; // Tiempo de retardo
D       = 1 / (1 + Td * s + Td**2 * s**2 /2);

G2      = G * D;
kp      = 40*60;
//kp      = 0.32

//Ti = 100;
Ti = 50;
//Ti = 1999/60;
cpi = (1/(Ti*s)+1);

T = (kp * cpi * G2) / (1 + kp * cpi * G2);
t = 0:1:1000;
//t = 0:0.001:2000;
salida = 85 * csim('step', t, T); 

close();
close();
evans(G2 * cpi,90); // / s es el integrador
plot([-1;0],[0.75;0],'-');
figure();
plot(t, salida);




