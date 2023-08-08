fid = fopen('review1.dat');
if fid == -1
    disp('file did not open')
end
aline = fgetl(fid);
count = 0;
while aline ~= -1
    count = count + 1;
    subplot(1,2,count)
    bar(str2num(strrep(aline,'x',' ')))
    aline = fgetl(fid);
end
fclose('all');
