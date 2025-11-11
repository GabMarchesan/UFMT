test.cow:
	@wget https://raw.githubusercontent.com/Mikhail57/cow-interpreter/master/test.cow


interpreter.py:
	@wget https://raw.githubusercontent.com/Mikhail57/cow-interpreter/master/interpreter.py


install: test.cow interpreter.py


hello: interpreter.py test.cow
	@python3 interpreter.py test.cow


clean:
	@rm -rf test.cow 
	@rm -rf interpreter.py


run: install hello 