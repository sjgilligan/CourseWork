function writel(varargin)
    if nargin == 1
        fprintf('%.2f',varargin{1});
    elseif nargin == 2
        fid = fopen(varargin{2},'w');
        fprintf(fid,'%.2f',varargin{1});
        fclose('all');
    end
end
        
        