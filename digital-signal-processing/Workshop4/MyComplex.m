classdef MyComplex
    properties
        real
        imag
    end
    
    methods
        function obj = MyComplex(real, imag)
            % Constructor
            obj.real = real;
            obj.imag = imag;
        end
        
        function result = mtimes(obj, other)
            % Overloaded multiplication operator
            result = MyComplex(obj.real * other.real - obj.imag * other.imag, ...
                             obj.real * other.imag + obj.imag * other.real);
        end
        
        function r = getReal(obj)
            % Get the real part
            r = obj.real;
        end
        
        function i = getImag(obj)
            % Get the imaginary part
            i = obj.imag;
        end
    end
end