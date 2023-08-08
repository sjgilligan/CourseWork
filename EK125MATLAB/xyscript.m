fid = fopen('xypoints.dat');
while fid == -1
    disp('File does not exist')
end
x = zeros(1,10);
y = zeros(1,10);
for i = 1:10
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

    
    