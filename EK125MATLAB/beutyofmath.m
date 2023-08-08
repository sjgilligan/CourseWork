x = 1;
for n = 1:8
    fprintf('%d x 8 + %d = %d \n',x,n,x*8+n)
    x = x *10 + n + 1;
end