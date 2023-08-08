x = rand * 10 - 5;
fprintf('%.2f',x)
choice = menu('choose an option','ceil','round','sign');
if choice > 0
    if choice == 1
        x = ceil(x);
    else
    end
    if choice == 2
        x = round(x);
    else
    end
    if choice == 3
        x = sign(x);
    end
end
fprintf('%d',x)
        