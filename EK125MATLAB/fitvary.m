function fitvary(x,y,guess)
    rng = (min(x):.1:max(x));
    hold on
    plot(x,y,'ko')
    plot(rng,polyval(guess,rng))
    plot(rng,polyval(polyfit(x,y,2),rng))
    legend('points','Your guess','polyfit')
end