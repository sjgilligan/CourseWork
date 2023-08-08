tsunamis = readtable('tsunamis.xlsx');
count = 0;
x = []
y = []
for i = 1:height(tsunamis)
    if tsunamis.ValidityCode(i) == 4 && tsunamis.CauseCode(i) == 1
        if isnan(tsunamis.EarthquakeMagnitude(i)) == 0 && isnan(tsunamis.Intensity(i)) == 0
            count = count + 1
            x(count) = tsunamis.EarthquakeMagnitude(i);
            y(count) = tsunamis.Intensity(i);
        end
    end
end
plot(x,y,'*')     