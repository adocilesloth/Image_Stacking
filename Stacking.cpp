#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "SFML\Graphics\Image.hpp"

sf::Image do_stacking(std::vector<sf::Image> &stack);

int main(int argc, char *argv[])
{
	std::cout << "Stacking - v1 - W.J.Pearson 2018" << std::endl;
	std::cout << "Loading Images...";
	std::vector<sf::Image> stack;
	sf::Image image;

	if(argc == 1)
	{
		std::cout << "no images found!" << std::endl;
		std::cout << "Drag images onto the .exe to stack them." << std::endl;
		std::cin.ignore();
		return 0;
	}

	std::cout << std::endl;
	for(int i = 1; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
		image.loadFromFile(argv[i]);
		stack.push_back(image);
	}

	std::cout << "Stacking...";
	sf::Image stacked_image = do_stacking(stack);
	std::cout << "Done" << std::endl;

	//Curate file path and extension
	std::string save_file = argv[1];
	size_t end = save_file.find_last_of(".");
	std::string extn = save_file.substr(end);
	end = save_file.find_last_of("\\/");
	save_file = save_file.substr(0, end+1);
	end = save_file.find_last_of(".");

	//get time as yyyymmddhhmmss
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer [20];
	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer,20,"%Y-%m-%d-%H-%M-%S",timeinfo);

	//set filename
	save_file += "Stacked_";
	save_file += buffer;
	save_file += extn;

	std::cout << "Saving to: " << save_file<< std::endl;
	stacked_image.saveToFile(save_file);
	std::cout << "Stacking Complete!" << std::endl;
	std::cout << "Press ENTER to exit" << std::endl;

	char GET;
	std::cin.ignore();

	return 0;
}

sf::Image do_stacking(std::vector<sf::Image> &stack)
{
	sf::Image stacked_image;

	stacked_image.create(stack[0].getSize().x, stack[0].getSize().y, sf::Color::Black);
	int ten_pct = stack[0].getSize().x / 10;
	int count = 10;

	for(int i = 0; i < stack[0].getSize().x; i++)
	{
		if(i % ten_pct == 0 && i != 0)
		{
			std::cout << count << "...";
			count += 10;
		}
		for(int j = 0; j < stack[0].getSize().y; j++)
		{
			std::vector<int> r, g, b, a;
			size_t size = stack.size();

			for(int k = 0; k < size; k++)
			{
				r.push_back(stack[k].getPixel(i, j).r);
				g.push_back(stack[k].getPixel(i, j).g);
				b.push_back(stack[k].getPixel(i, j).b);
				a.push_back(stack[k].getPixel(i, j).a);
			}

			sort(r.begin(), r.end());
			sort(g.begin(), g.end());
			sort(b.begin(), b.end());
			sort(a.begin(), a.end());

			int red, gre, blu, alp;
			if(size % 2 == 0)
			{
				red = (r[size / 2 - 1] + r[size / 2]) / 2;
				gre = (g[size / 2 - 1] + g[size / 2]) / 2;
				blu = (b[size / 2 - 1] + b[size / 2]) / 2;
				alp = (a[size / 2 - 1] + a[size / 2]) / 2;
			}
			else
			{
				red = r[size / 2];
				gre = g[size / 2];
				blu = b[size / 2];
				alp = a[size / 2];
			}

			stacked_image.setPixel(i, j, sf::Color(red, gre, blu, alp));
		}
	}

	return stacked_image;
}