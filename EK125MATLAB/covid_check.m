function data = covid_check(code)
% Help: covid_check('US') returns relevant COVID-19 data about a country
% using its 2-character country code.

% Postman Covid-19 endpoint
endpoint = "https://api.covid19api.com/summary";

% Webread makes an HTTP GET request to Postman's endpoint.
% The only parameter for this API call is the endpoint (or URL).

resp = webread(endpoint);

% Run this code in matlab and observe the output. It should return a data
% structure you are familiar with. Using this data structure, figure out
% how to extract the country name, total confirmed cases, and total deaths
% and return it in a structure.

% For the function output, create a struct with 3 fields:
% 'Name', 'TotalConfirmed' and 'TotalDeaths'
% start your code here:
for i = 1:height(resp.Countries)
   if resp.Countries(i).CountryCode == code
       data = struct(Name = resp.Countries(i).Country,TotalConfirmed = resp.Countries(i).TotalConfirmed,TotalDeaths = resp.Countries(i).TotalDeaths);
   end
end
 
end
