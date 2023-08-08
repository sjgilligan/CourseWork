% creating the file parts_inv.dat. Do not change the following lines:
qwerty = fopen('parts_inv.dat','w');
for i = 1:randi([10,20]) fprintf(qwerty,'%d %.2f %d\n',randi([100,200]),rand()*10+5,randi([20,100])); end
fclose('all');
  
% read the file 'parts_inv.dat' into partsmat using fgetl
fid = fopen('parts_inv.dat');
aline = fgetl(fid);
count = 0;
while aline ~= -1
    aline = str2num(aline);
    count = count + 1;
    partsmat(count,:) = aline;
    aline = fgetl(fid);
end
