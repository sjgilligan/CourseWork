a = input('input a: ');
c = input('input c: ');
if a <= 0 || c <= 0
    error('invalid inputs')
end
b = findb(a,c);
fprintf('b is %.2f',b)

function b = findb(a,c)
% Calculates b from a and c
b = sqrt(c^2 - a^2);
end