% proc_msg

p = play(r);

org_data = getaudiodata(r);   % get the sample data as an array
plot(org_data);

% Eliminating silent parts at the beginning and end
start = 3254;
stop = 13150;

plot(org_data(start:stop));
p = play(r, [start stop]);

s_data = org_data(start:stop);        % short version
s_data_n = s_data/max(abs(s_data));   % Normalized short version
sound(s_data_n, fs, 16);

data = uint8(127*s_data_n+128);
plot(data);

save('new_msg.mat', 'r', 'org_data', 'data');

gen_include('msg_data.h', data, 'msg_data')

