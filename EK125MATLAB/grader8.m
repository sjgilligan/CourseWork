% create vector of structures named subjects with potential subjects
% for the experiment (see example above). Calculate  the average height
% and weight, then determine and print eligible participants
subjects = [struct(name = 'Joey',sub_id = '111',height = 6.7,weight = 222.2),struct(name = 'Fred',sub_id = '222',height = 5.3,weight = 120.3),struct(name = 'Ned',sub_id = '333',height = 5.9,weight = 180.5)];
%calculate the average height and weight
sumheight = 0;
sumweight = 0;
for i = 1:length(subjects)
    sumheight = sumheight + subjects(i).height;
    sumweight = sumweight + subjects(i).weight;
end
avgheight = sumheight/length(subjects);
avgweight = sumweight/length(subjects);

%find and print the names of the eligible participants. Also store them in the nameArray cell array
count = 0;
for i = 1:length(subjects)
    if subjects(i).height < avgheight || subjects(i).weight < avgweight
        count = count + 1;
        nameArray{count} = subjects(i).name;
    end
end