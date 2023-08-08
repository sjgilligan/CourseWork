cm2inch = @cmtoinches;
meter2feet = @meterstofeet;
mile2feet = @milestofeet;
save lenconv.mat cmtoinch meter2feet mile2feet;

function inches = cmtoinches(cm)
    inches = cm * 2.54;
end
function feet = meterstofeet(meters)
    feet = meters * 3.28084;
end
function feet = milestofeet(miles)
    feet = miles * 5280;
end
    