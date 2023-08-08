function [root1 root2] = quadroots(a,b,c)
    root1 = (-b + sqrt(calcdisc(a,b,c)))/(2*a);
    root2 = (-b - sqrt(calcdisc(a,b,c)))/(2*a);
    function D = calcdisc(a,b,c)
        D = b^2-4*a*c;
    end
end

