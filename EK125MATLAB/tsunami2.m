tsunamis = readtable('tsunamis.xlsx');
count = 0;
lat = [];
long = [];
inten = [];
for i = 1:height(tsunamis)
    if tsunamis.ValidityCode(i) == 4 && tsunamis.CauseCode(i) == 1
        if isnan(tsunamis.EarthquakeMagnitude(i)) == 0 && isnan(tsunamis.Intensity(i)) == 0
            count = count + 1;
            inten(count) = tsunamis.Intensity(i);
            lat(count) = tsunamis.Latitude(i);
            long(count) = tsunamis.Longitude(i);
        end
    end
end
geobubble(lat,long,inten)