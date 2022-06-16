SRC		=	$(addprefix src/, main.cpp Parser.cpp Server.cpp ServerParameters.cpp)
OBJDIR	=	obj
SRCDIR	=	src
OBJ		=	$(addprefix  obj/, $(notdir  $(SRC:.cpp=.o)))

NAME	=	webserv
CC		=	c++
#-Wall -Wextra -Werror # 
CFLAGS	=	 -MD -MP -g -std=c++98 -I include
DEPENDS :=	$(addprefix  obj/, $(notdir  $(SRC:.cpp=.d)))

.PHONY	:	all re clean fclean

all		:	$(NAME)

$(NAME)	:	$(OBJ) Makefile
			$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o		:	$(SRCDIR)/%.cpp
			$(CC) $(CFLAGS) -c -o $@ $<

clean	:	
			rm -f $(OBJ) $(DEPENDS)

fclean	:	clean
			rm -f $(NAME)
			rm -rf null.d
			

x		:	all
			./$(NAME) default.conf

leaks	:	all
			leaks --atExit -- ./$(NAME)
			
$(OBJ)			: | $(OBJDIR)

$(OBJDIR)		: 
					mkdir $(OBJDIR)

re		:	fclean all

-include $(DEPENDS)