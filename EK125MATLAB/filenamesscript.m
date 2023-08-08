fid = fopen('potfilenames.dat','w');
ca = {'hello', 'goodbye', 'tree', 'dirt'};
fprintf(fid,'hello\ngoodbye\ntree\ndirt\nbuilding');
fclose(fid);
fid = fopen('potfilenames.dat');
aline = fgetl(fid);
count = 0;
while aline ~= -1
    disp(strcat(aline,'.dat'));
    count = count + 1;
    aline = fgetl(fid);
end
fclose(fid);
fprintf('%d file names were valid',count)
