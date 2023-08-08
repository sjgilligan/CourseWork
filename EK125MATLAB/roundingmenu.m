x = rand * 10 - 5;
fprintf('%.2f',x)
choice = menu('choose an option','ceil','round','sign');
switch choice > 0
    case choice == 1
        x = ceil(x);
    case choice == 2
        x = round(x);
    case choice == 3
        x = sign(x);
end
fprintf('%d',x)
        
    
