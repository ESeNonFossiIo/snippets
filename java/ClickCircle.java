
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;

/*
Permette di disegnare e cancellare cerchi. 
Usa i bottoni per scegliere l'operazione.
Per inserire clicca sul punto che sara' il centro.
Per cancellare clicca sul cerchio.
*/

public class ClickCircle extends Panel 
{

/* bottoni a due stati, mutuamente esclusivi, per l'operazione
	da eseguire con il click */
Checkbox insert_but;
Checkbox delete_but;

/* raggio di un cerchio */
int r;

/* massimo numero di cerchi ammessi */
static final int MAX_NUM = 30;

/* array di cerchi presenti e loro numero */
Ellipse2D[] circle_list;
int circle_num;

public ClickCircle()
{
	/* crea i due bottoni in un gruppo in modo che siano esclusivi */
	CheckboxGroup g = new CheckboxGroup();
	insert_but = new Checkbox ("Inserisci",g,true);
	delete_but = new Checkbox ("Cancella",g,false);
	/* stabilisce situazione iniziale per i cerchi */
	circle_list = new Ellipse2D[MAX_NUM];
	circle_num = 0;
	r = 10;
	/* rende il pannello sensibile al click del mouse in modo che
	reagisca guardando quale dei due bottoni e selezionato e 
	invocando inserimento o cancellazione secondo il caso */
	addMouseListener( new MouseAdapter()
	{   public void mouseClicked(MouseEvent e)
		{
		if ( insert_but.getState()==true )
			insertCircle(e.getX(),e.getY());
		else deleteCircle(e.getX(),e.getY()); 
		}
	});
}

/* Disegna i cerchi presenti in rosso */
public void paint(Graphics g)
{
	int i;
	super.paint(g);
	g.setColor(Color.red);
	Graphics2D g2 = (Graphics2D)g;
	for (i=0; i<circle_num; i++)  g2.fill(circle_list[i]);
}

/* inserisce nuovo cerchio alla posizione indicata e ridisegna */
public void insertCircle(int x, int y)
{
	if (circle_num==MAX_NUM-1)
	System.out.println("Troppi cerchi, non posso inserire");
	else
	{  Ellipse2D ell = new Ellipse2D.Float(x-r,y-r,2*r,2*r);
	circle_list[circle_num++] = ell;
	}
	repaint();
}

/* Cancella cerchio su cui cade (x,y), se esiste, e ridisegna */
public void deleteCircle(int x, int y)
{
	int i, j;
	boolean deleted = false;
	for (i=0; (i<circle_num)&&(!deleted); i++)
	{  if ( circle_list[i].contains(x,y) )
	{  /* cancella cerchio dall'array spostando tutti i seguenti */
		for (j=i+1; j<circle_num; j++)
		{  circle_list[j-1] = circle_list[j];
		}
		circle_num--;
		deleted = true;
	}
	}
	if (deleted) repaint();
}

/* crea frame che ospita il pannello e i suoi due bottoni */
public static Frame createFrame(int dimX, int dimY)
{
	Frame f = new Frame("Click Circle");
	f.setLayout(new BorderLayout());
	/* mette al centro il pannello grafico */
	ClickCircle cc = new ClickCircle();
	cc.setBackground(Color.white);
	f.add(BorderLayout.CENTER,cc);
	/* mette in alto il pannello coi due bottoni */
	Panel p = new Panel();
	p.setLayout( new FlowLayout() );
	p.add(cc.insert_but);
	p.add(cc.delete_but);
	f.add(BorderLayout.NORTH,p);
	/* stabilisce dimensioni */
	f.setSize(dimX,dimY);
	return f;
}

public static void main(String s[])
{
	Frame f = ClickCircle.createFrame(300,300);
	f.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e)
			{System.exit(0);}
		});
	f.show();
}


}