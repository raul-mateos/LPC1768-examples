% import_adc.m
% import_adc('msg_data.h', data, 'msg_data')
  
function x = import_adc(file_name)
  
  x = [];
  fid = fopen(file_name, 'r');
  if (fid == -1) 
    error('Error while openning file ''%s''', file_name); 
  end
  
  x = fread(fid, inf, 'uint16=>uint16');
  fclose(fid);
  
  
  n = length(x);

% file_name = 'C:\temp\acq.bin'
% x = import_adc('C:\temp\acq.bin')

