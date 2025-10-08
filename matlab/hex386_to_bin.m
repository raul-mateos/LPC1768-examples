% hex386_to_bin.m
% hex386_to_bin('adc_hw_int_cnv', '0x10000018', '0x10000817');

function x = hex386_to_bin(hex386_file, txt_start_addr, txt_end_addr)

x = [];

utils_path = 'C:/asignaturas/SED_GIEAI/projects/utils/srecord-1.65.0-win64/bin';

start_addr = sscanf(txt_start_addr,'0x%X');
end_addr = sscanf(txt_end_addr,'0x%X');

hex386_file = '../adc_hw_int_cnv/acq_download.hex';    
[hex386_dir, hex386_name, hex386_ext] = fileparts(hex386_file);
output_file = strcat(hex386_name, '.bin');
output_file_path = fullfile(hex386_dir, output_file);
cnv_util_path = fullfile(utils_path, 'srec_cat.exe');

cmd_txt = sprintf('%s %s -Intel -crop 0x%8.8X 0x%8.8X -offset -0x%8.8X -o %s -Binary', ...
            cnv_util_path,  ...
            hex386_file,    ...
            start_addr,     ...
            end_addr+1,     ...
            start_addr,     ...
            output_file_path);

status = system(cmd_txt);

if (status == 0)
  x = import_adc(output_file_path);
end

