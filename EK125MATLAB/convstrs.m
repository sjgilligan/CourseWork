function out = convstrs(charmat,uorl)
    if uorl == 'u'
        out = upper(charmat);
    elseif uorl == 'l'
        out = lower(charmat);
    else
        out = charmat;
end

