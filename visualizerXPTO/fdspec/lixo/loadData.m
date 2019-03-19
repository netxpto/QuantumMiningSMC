function data=loadData(fileName)

fid = fopen(fileName,'r');

offSetValue = 0;
while true
    str = fgetl(fid);
    offSetValue = offSetValue + length(str) + 2;
    if (strcmp(str, '// ### HEADER TERMINATOR ###'))
        break;
    end
end

fclose(fid);

m=memmapfile(fileName,'Offset',offSetValue,'Format','double');

data = m.data;

end