lbase = input('Enter the length of the base: ');
units = input('Is that i or c ','s');
if units == 'c'
    lbase = lbase/2.54;
end
wbase = input('Enter the width of the base: ');
units = input('Is that i or c ','s');
if units == 'c'
    wbase = wbase/2.54;
end
height = input('Enter the height of the pyramid: ');
units = input('Is that i or c ','s');
if units == 'c'
    height = height/2.54;
end
fprintf('The volume of the pyramid is %.3f inches cubed',(1/3)*(lbase*wbase)*height)
