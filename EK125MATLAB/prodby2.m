function oddprod = prodby2(n)
    if round(n/2)==n/2
            oddprod = prod(1:2:(n-1))
    else
            oddprod = prod(1:2:n)  
end

