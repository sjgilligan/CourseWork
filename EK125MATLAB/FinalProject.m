tempname = "Acadia National Park";
percvec = [];
nativetot = 0;
nonnativetot = 0;
count = 1;
for (i = 1:height(species))
    if (species.Nativeness == Native)
        nativetot = nativetot + 1;
    else if (species.Nativeness == Nonnative)
        nonnativetot = nonnativetot + 1;
    end
    percvec(count) = nonnativetot/(nativetot + nonnativetot);
    if (tempname ~= string(species.ParkName{i}))
        tempname = string(species.ParkName{i})
        count = count + 1;
        nativetot = 0;
        nonnativetot = 0;
    end
end
