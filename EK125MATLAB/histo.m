vec1 = randi([1 5],1,20);
vec2 = randi([1 500],1,20);
fprintf('Vec1, min: %d, max: %d, mean: %.2f, median: %.1f, std: %.3f, mode: %d\n',...
    min(vec1),max(vec1),mean(vec1),median(vec1),std(vec1),mode(vec1))
fprintf('Vec2, min: %d, max: %d, mean: %.2f, median: %.1f, std: %.3f, mode: %d\n',...
    min(vec2),max(vec2),mean(vec2),median(vec2),std(vec2),mode(vec2))
subplot(1,2,1)
histogram(vec1)
subplot(1,2,2)
histogram(vec2)