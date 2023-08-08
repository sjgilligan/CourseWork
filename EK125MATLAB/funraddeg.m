dbstop 3
degrees = prompt;
radians = calc(degrees);
print(radians);
function degrees = prompt
    degrees = input('Enter degrees: ');
end
function radians = calc(degrees)
    radians = (degrees/180)*pi;
end
function print(radians)
    disp(radians);
end
