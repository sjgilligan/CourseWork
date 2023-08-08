for i = 1:3
    lenf = lenprompt();
    leni = convertFtToIn(lenf);
    printLens(lenf, leni)
end
function lenf = lenprompt
    lenf = 12;
end
function leni = convertFtToIn(lenf)
    leni = 5;
end
function printLens(lenf, leni)
end