import java.awt.*;
import java.awt.event.*;
import javax.swing.Timer;

/* 
Muove un cerchio da sinistra a destra, quando cozza la parete 
destra il moto si inverte ecc.
*/

class MuoviCerchio extends Panel implements ActionListener
{

int raggio;          /* raggio del cerchio */
int millisec;        /* ogni quanti millisecondi si muove */
Timer timer;         /* per regolare l'animazione */
boolean versoDestra; /* da che parte va */
int x_min;           /* ascissa minima corrente del cerchio */

public MuoviCerchio (int raggio_cerchio, int velocita)
{
	raggio = raggio_cerchio;
	millisec = velocita;
	x_min = 0;  /* cerchio appoggiato a parete sinistra */
	versoDestra = true; /* direzione di moto */
	timer = new Timer(millisec,this);
	timer.start();
}

/* azione da compiere quando scatta il timer, sposta il cerchio
	e poi invoca il ridisegno */
public void actionPerformed(ActionEvent e)
{
	Dimension d = getSize();
	if (versoDestra)
	{  
	if (x_min>(d.width-2*raggio)) /* cozza parete destra */
	{  versoDestra = false;
		x_min--;
	}
	else x_min++;
	}
	else
	{
	if (x_min<=0) /* cozza parete sinistra */
	{  versoDestra = true;
		x_min++;
	}
	else x_min--;
	}
	repaint();
}

public void paint(Graphics g)
{
	super.paint(g);
	Dimension d = getSize();
	g.setColor( Color.blue );
	g.fillArc(x_min, (d.height/2-raggio), 2*raggio, 2*raggio, 0, 360);
}

public static void main(String[] args)
{
	Frame f = new Frame("MuoviCerchio");
	MuoviCerchio pp = new MuoviCerchio(30,50);
	pp.setBackground(Color.white);
	f.add(pp);
	f.setSize(new Dimension(300,100));
	f.setVisible(true);
}
}