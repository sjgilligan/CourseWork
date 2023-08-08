function out = filemaker
    filename = input('Enter a filename: ','s');
    extension = input('Enter a file extension: ','s');
    out = strcat(filename,'.',extension);
end

