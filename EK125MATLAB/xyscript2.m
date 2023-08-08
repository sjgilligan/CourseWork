fid = fopen('xypoints.dat');
while fid == -1
    disp('File does not exist')
end
subjdata = textscan(fid,'%s %f %s %f');
x = zeros(1,height(subjdata{1}));
y = zeros(1,height(subjdata{1}));
for i = 1:length(subjdata{1})
    aline = fgetl(fid);
    [first last] = strtok(aline,'y');
    first = erase(first,'x');
    last = erase(last,'y');
    x(i) = str2num(first);
    y(i) = str2num(last);
end
plot(x,y,'*')
close = fclose(fid);
if close == 0
    disp('File closed')
end
