fs = 120;
T  = 1/fs;
R  = 1e3;
C  = 3.18e-6;

a = exp(-T/(R*C));
b = 1 - a;          % correct feedforward coefficient

z = tf('z', T);    % discrete z variable with sample time T
H = b / (1 - a*z^-1);

%bode(H);
%grid on;

% specs
fs = 200;
fp = 50;
fsb = 55;
Rp = 1;   % dB passband ripple
Rs = 20;  % dB stopband attenuation

% digital normalized freqs
Wp = fp/(fs/2);
Ws = fsb/(fs/2);

% find min order & critical freq (digital)
[n, Wn] = ellipord(Wp, Ws, Rp, Rs);

% digital design
[b, a] = ellip(n, Rp, Rs, Wn, 'low')

% display
fprintf('Elliptic order = %d\n', n);
[sos, g] = tf2sos(b, a);
disp('Second-order sections (sos):'); disp(sos);

% plot frequency response
figure; freqz(b, a, 2048, fs); title('Digital elliptic IIR (freqz)'); grid on;

% safer implementation for filtering:
% y = sosfilt(sos, x);

% -- if you prefer the analog prototype -> prewarp -> tustin route:
T = 1/fs;
Wp_analog = (2/T) * tan(pi*fp/fs);
% use ellipap to get prototype
[z_a, p_a, k_a] = ellipap(n, Rp, Rs);
% scale prototype
p_as = p_a * Wp_analog;
z_as = z_a * Wp_analog;
k_as = k_a * Wp_analog^n;
[num_a, den_a] = zp2tf(z_as, p_as, k_as);
Ha = tf(num_a, den_a);
Hd_tustin = c2d(Ha, T, 'tustin');
figure; bode(Hd_tustin); grid on; title('Analog prototype -> Tustin');

