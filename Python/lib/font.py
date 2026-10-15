def bold(msg):
  """
    Print msg in bold font.
  """
	return u'\033[1m%s\033[0m' % msg

def color(this_color, string):
	"""
    - this_color is a number in the range (30,37) and represents the color
      of the text;
    - string is the content of the text.
    A possible test is:
	    >>> for i in range(30, 38):
	    >>> c = str(i)
	    >>> print('This is %s' % color(c, 'color ' + c))
	"""
	return "\033[" + this_color + "m" + string + "\033[0m"
