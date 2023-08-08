%prompt the user for an angle in degrees
ang = input("Enter an angle in degrees: ");
%convert degrees to radians
ang = (ang*pi)/180;
%display result
fprintf('The angle in radians is %.2f',ang)