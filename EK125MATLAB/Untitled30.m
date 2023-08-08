charrep('*',6)
function outstring = charrep(thechar,num)
    charvec = blanks(num);
    outstring = strrep(charvec,' ',thechar);
end