% gen_include.m
% gen_include('msg_data.h', data, 'msg_data')
  
function n = gen_include(file_name, x, var_name)

  fid = fopen(file_name, 'w');
  if (fid == -1) 
    error('Error while openning file ''%s''', file_name); 
  end
  
  n = length(x);
  
  var_name_h = strcat(upper(var_name), '_H');

  x_int = uint8(x);
  
  fprintf(fid, '#ifndef %s\n', var_name_h);
  fprintf(fid, '#define %s\n', var_name_h);
  fprintf(fid, '\n');
  fprintf(fid, '#include <stdint.h>\n');
  fprintf(fid, '\n');
  fprintf(fid, '#define %s %d\n', upper(strcat(var_name, '_LEN')), n);
  fprintf(fid, '\n');
  fprintf(fid, 'uint8_t %s[] = {\n', var_name);

  for i=1:n-1
    sample = x_int(i);
    fprintf(fid, '    0x%2.2X,    // value = %4d\n', sample, sample);
  end
  sample = x_int(n);
  fprintf(fid, '    0x%2.2X};   // value = %4d\n', sample, sample);
  
  fprintf(fid, '\n');
  fprintf(fid, '#endif // %s\n', var_name_h);
  
  fclose(fid);

