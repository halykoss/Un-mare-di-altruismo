import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(name='Un mare di altruismo',
                 version='1.0',
                 description=long_description,
                 author='Filippo Mei, Federico Pennino, Italo Perrucci',
                 author_email='gward@python.net',
                 url='https://www.python.org/sigs/distutils-sig/',
                 packages=setuptools.find_packages(),
                 install_requires=["matplotlib"]
                 )
