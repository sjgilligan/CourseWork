function varargout = rectangleorbox(varargin)
        area = varargin{1} * varargin{2};
        varargout{1} = area;
    if nargin == 3
        volume = varargin{1} * varargin{2} * varargin{3};
        varargout{2} = volume;
    end
    
  
   
        