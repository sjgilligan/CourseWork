function interqrange = quartiles(vec)
    set = sort(vec);
    Q2 = median(set);
    Q1 = median(set(1:(length(set)/2)));
    Q3 = median(set((length(set)/2):length(set)));
    interqrange = Q3 - Q1;
end

