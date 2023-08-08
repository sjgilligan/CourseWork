function caprint(ca)
    er = 0;
    for i = 1:length(ca)
        if ischar(ca{i}) == false
            error('not all char vecs')
            er = er + 1;
        end
    end
    if er ~= 1
        for j = 1:length(ca)
            fprintf('CV %d is: %s\n',j,ca{j})
        end
    end     
end

