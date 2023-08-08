function funthree(thechar,varargin)
    if nargin == 2
        for i = 1:varargin{1}
            disp(strrep(blanks(varargin{1}),' ',thechar))
        end
    elseif nargin == 3
        for i = 1:varargin{1}
            disp(strrep(blanks(varargin{2}),' ',thechar))
        end
    end
end

