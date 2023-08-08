J = 4; %number of joints
%M = 2.*J - 3; %number of members
M = 5;
W = 32;
length = 0;
lengths_vec = zeros(1,M);

%C = zeros(J,M);%connections
C = [1 1 0 0 0;
     1 0 1 1 0;
     0 1 1 0 1;
     0 0 0 1 1]
     
      
Sx = zeros(J,3); %reactions
Sx(1,1) = 1;
Sy = zeros(J,3);
Sy(1,2) = 1;
Sy(4,3) = 1;

X = zeros(1,J); %locations
Y = zeros(1,J);
X = [0,0,11,0];
Y = [0,11,11,22];

L = zeros(2*J,1); %load
L(2+J) = W;

A = zeros(2*J,M);

for i = 1:1:M
  iterate = 1;
  Xcoord = zeros(2,2);
  Ycoord = zeros(2,2);
    for k = 1:1:J
        if C(k,i) == 1 
            Xcoord(iterate,1) = X(k);
            Ycoord(iterate,1) = Y(k);
            Xcoord(iterate,2) = k;
            Ycoord(iterate,2) = k;
            iterate = iterate + 1;
        end      
    end
    
    r = sqrt((Xcoord(1,1) - Xcoord(2,1)).^2 + (Ycoord(1,1) - Ycoord(2,1)).^2);
    length = length + r;
    cosine = (Xcoord(1,1) - Xcoord(2,1))./r;
    sine = (Ycoord(1,1) - Ycoord(2,1))./r;
    A(Xcoord(1,2),i) = cosine;
    A(Xcoord(2,2),i) = -1.*cosine;
    A(Xcoord(1,2) + J,i) = sine;
    A(Xcoord(2,2) + J,i) = -1.*sine;
    lengths_vec(i) = r;
end
S = cat(1,Sx,Sy);
A = cat(2,A,S);
A
T = inv(A)*L;

fprintf("EK301, Section A3, Group 5: Kevin Chen, Sebastian Gilligan, Daniel Juweid");
fprintf("\nLoad = %d oz",W);
fprintf("\nMember Forces in oz");
for i = 1:M
    if T(i) >= 0
        Character = 'T';
    else
        Character = 'C';
    end
    max_load = -4338.*(lengths_vec(i).^-2.125);
    fprintf("\nm%d: %f (%c), max load: %f", i, abs(T(i)), Character, max_load);
    fprintf(" length: %f", lengths_vec(i))
end
fprintf("\nReaction forces in oz:");
fprintf("\nSx1: %f\nSy1: %f\nSy2: %f",T(M+1),T(M+2),T(M+3));
price = J.*10 + length;
fprintf("\nCost of truss: $%.2d",price);
fprintf("\nTheoretical max load/cost ratio in oz/$: %f",38.5./price);