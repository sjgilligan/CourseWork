experiments = [struct(num = randi(99),name = 'Joe',weights = 100 * rand,...
    height = struct(feet = randi(6),inches = randi(11))), struct(num = randi(99)...
    ,name = 'Kevin',weights = 100 * rand, height = struct(feet = randi(6),...
    inches = randi(11)))]
printhts(experiments);
heighttotal = howhigh(experiments)
function printhts(in)
    for i = 1:length(in)
        fprintf('%s, %d feet, %d inches\n',in(i).name,in(i).height.feet,in(i).height.inches)
    end
end
function out = howhigh(in)
    out = 0;
    for i = 1:length(in)
        out = out + (12*in(i).height.feet) + in(i).height.inches;
    end
end