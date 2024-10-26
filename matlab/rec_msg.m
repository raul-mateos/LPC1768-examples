% rec_msg

fs = 8000;      % sampling frequency = 8 KHz
n_bits = 16;    % samples coded with 16 bits
t_len = 2;      % length = 2 seconds

r = audiorecorder(fs, n_bits, 1);
record(r);      % speak into microphone...
fprintf(1, 'speak into microphone...\n');
pause(t_len);
stop(r);
