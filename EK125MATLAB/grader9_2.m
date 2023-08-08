% create vectors x, y, and z with random real numbers:
len = randi(50);
x = rand(1,len);
y = rand(1,len);
z = rand(1,len);

% create the file three-d.dat using fprintf:
fid = fopen('three-d.dat','w');
for i = 1:len
    fprintf(fid,'x %f y %f z %f\n',x(i),y(i),z(i));
end
fclose('all');