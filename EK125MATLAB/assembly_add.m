function result = assembly_add(thestring)
    [func nums] = strtok(thestring)
    [first second] = strtok(nums, ',')
    second = strrep(second, ',',' ')
    first = str2num(first)
    second = str2num(second)
    result = first + second
end

