anum = randi([3,10]);
thedata = [randi([1,350],anum,1),rand(anum,1)*5,rand(anum,1)*15];
save mydata.dat thedata -ascii
structure = structmaker();
tablemaker(structure);
avgintensity(structure);
mostinten(structure);
function outstruct = structmaker()
    load mydata.dat
    for i = height(mydata):-1:1
    outstruct(i) = struct(code = mydata(i,1),amount = mydata(i,2),duration = mydata(i,3),...
        intensity = (mydata(i,2)/mydata(i,3)));
    end
end
function tablemaker(instruct)
    disp(struct2table(instruct))
end
function avgintensity(in)
    intensum = 0;
    for i = 1:length(in)
        intensum = intensum + in(i).intensity;
    end
    fprintf('Average Intensity: %.3f\n',intensum/length(in));
end
function mostinten(in)
    highest = 0;
    for i = 1:length(in)
        if in(i).intensity > highest
            highest = in(i).intensity;
        end
    end
    for i = 1:length(in)
        if in(i).intensity == highest
            disp(struct2table(in(i)))
        end
    end
end