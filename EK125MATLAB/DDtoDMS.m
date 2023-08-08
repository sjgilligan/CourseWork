DD = input('Enter decimal degrees: ');
while DD < 0 || DD > 360
    error('Wrong input');
    DD = input('Enter decimal degrees: ');
end
[D M S] = calcDMS(DD)
function [D M S] = calcDMS(DD);
    D = floor(DD);
    M = 60*(DD - D); 
    S = 60 * mod(M,floor(M));
    M = floor(M);
end
    