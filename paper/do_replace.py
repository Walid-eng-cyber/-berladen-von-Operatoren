#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script to replace Sections 3 and 4 in seminararbeit.tex with expanded versions.
"""

# Read the full file
with open('seminararbeit.tex', 'r', encoding='utf-8') as f:
    lines = f.readlines()

# New Section 3 and Section 4 content
new_sections = r"""% -----------------------------------------------------------
\section{Experimente und Analyse}
\label{sec:experimente}

\subsection{Versuchsaufbau}
\label{sec:versuchsaufbau}

Da seit C++17 die Materialisierung eines prvalue-Rückgabewerts verpflichtend
ohne Kopie oder Move erfolgt, unterscheiden sich naive und move-fähige
Implementierungen im Fall der direkten Initialisierung (\texttt{Matrix D = A + B;})
in der Regel kaum. Reale Unterschiede sind dagegen bei der Zuweisung aus einem
temporären Objekt, bei verketteten Ausdrücken mit echten Zwischenwerten sowie
bei Container-Reallokationen zu erwarten. Der Versuchsaufbau ist entsprechend so
gewählt, dass die unterscheidenden Primitive der drei Versionen einzeln
gemessen werden:

\begin{enumerate}
  \item \textbf{Operationskosten:} Addition, Subtraktion, Multiplikation und
  verkettete Ausdrücke (Initialisierung aus prvalue).
  \item \textbf{Kopie vs.\ Move:} Kopier- gegen Move-Konstruktor sowie Kopier-
  gegen Move-Zuweisung (isoliert den Beitrag von Version~2 gegenüber Version~1).
  \item \textbf{Freie Funktion vs.\ in-place:} \texttt{A + B} gegen
  \texttt{A += B} über mehrere Matrixgrößen (isoliert den Beitrag von
  Version~3 gegenüber Version~2).
  \item \textbf{Skalierung:} Addition und Multiplikation über einen Größenbereich.
  \item \textbf{Container und Akkumulation:} Einfügen in \texttt{std::vector<Matrix>}
  sowie Akkumulationsschleifen.
  \item \textbf{Temporäre Objekte:} Zählung der Kopien und Moves je Operation.
\end{enumerate}

Sämtliche Laufzeitmessungen wurden auf der in Tabelle~\ref{tab:umgebung}
beschriebenen Umgebung durchgeführt. Für jede Konfiguration werden identische
Eingangsdaten und Compiler-Flags verwendet, sodass beobachtete Unterschiede aus
den Implementierungsvarianten und nicht aus veränderten Randbedingungen stammen.

\begin{table}[htbp]
\centering
\caption{Mess- und Systemumgebung.}
\label{tab:umgebung}
\begin{tabular}{ll}
\toprule
\textbf{Komponente} & \textbf{Spezifikation} \\
\midrule
Prozessor    & 16 Kerne, 3800\,MHz \\
Cache        & L1 32\,KiB ($\times 8$), L2 512\,KiB ($\times 8$), L3 32\,MiB \\
Compiler     & Clang 22.1.6 (llvm-mingw) \\
Sprachstandard & C++17 \\
Messframework  & Google Benchmark 1.9.5 \\
Optimierung    & \texttt{-O0}, \texttt{-O2}, \texttt{-O3} \\
\bottomrule
\end{tabular}
\end{table}

\subsection{Messmethodik und Reproduzierbarkeit}
\label{sec:methodik}

Die Laufzeitmessung erfolgt mit Google Benchmark. Das Framework bestimmt die
Iterationszahl automatisch anhand einer Mindestlaufzeit
(\texttt{-{}-benchmark\_min\_time=0.05s}), führt mehrere Messläufe mit
wachsender Iterationszahl durch und aggregiert die Ergebnisse statistisch. Der
Aufruf \texttt{benchmark::DoNotOptimize} verhindert, dass der Compiler die zu
messende Berechnung als toten Code entfernt. Im erweiterten Benchmark werden die
Matrizen mit festem Zufallssamen ($\mathit{SEED}=42$) initialisiert, um
Reproduzierbarkeit sicherzustellen. Die Kopier- und Move-Zähler werden über die
in Abschnitt~\ref{sec:instrumentierung} beschriebene Instrumentierung erfasst.

Die in dieser Arbeit verwendete Profilierung des Speicherverhaltens stützt sich
auf diese Zähler und nicht auf externe Werkzeuge: \texttt{valgrind massif} und
\texttt{perf} standen auf der eingesetzten Windows-/llvm-mingw-Plattform nicht
zur Verfügung. Die in Abschnitt~\ref{sec:speicher} berichteten Kennzahlen sind
daher als Anzahl von Kopier- und Move-\emph{Operationen} zu verstehen.

\paragraph{Grenzen der Messung.}
Mehrere Mikrobenchmarks (Move-Konstruktor, Move-Zuweisung sowie die
in-place-Varianten bei kleinen Matrizen) kapseln ihre Vorbereitung in
\texttt{state.PauseTiming()}/\texttt{ResumeTiming()}. Das Pausieren und
Fortsetzen der Zeitmessung verursacht selbst einen Aufwand in der Größenordnung
von Mikrosekunden und dominiert daher Operationen, die -- wie ein Move -- nur
wenige Nanosekunden benötigen. Die entsprechenden Vergleiche
(Abschnitt~\ref{sec:interpretation}) sind deshalb nicht als belastbare Messung
der reinen Move-Kosten zu werten. Belastbar sind hingegen jene Messungen, die
ohne Pausieren auskommen: die Operations- und Skalierungsmessungen sowie der
Vergleich von freier Funktion und in-place-Operator bei größeren Matrizen.
Weitere Einschränkungen sind die Beschränkung auf eine einzige Maschine sowie
ein unvollständiger erweiterter \texttt{-O3}-Lauf.

\subsection{Ergebnisse: Laufzeit}
\label{sec:laufzeit}

Tabelle~\ref{tab:optlevels} zeigt die Laufzeit der Kernoperationen über die drei
Optimierungsstufen. Der Übergang von \texttt{-O0} zu \texttt{-O2} bringt den mit
Abstand größten Gewinn: Die Addition einer $100\times100$-Matrix beschleunigt
sich von rund 18{,}8\,$\mu$s auf 3{,}1\,$\mu$s (Faktor~6{,}2), die Multiplikation
von rund 796\,$\mu$s auf 61\,$\mu$s (Faktor~13). Der weitere Schritt zu
\texttt{-O3} bringt dagegen keinen konsistenten Vorteil; in mehreren Fällen ist
\texttt{-O3} sogar geringfügig langsamer (Abbildung~\ref{fig:optlevels}). Für die
speicherbandbreitenbegrenzte elementweise Addition liefert die zusätzliche
Vektorisierung bei \texttt{-O3} kaum Nutzen, während die Multiplikation bereits
bei \texttt{-O2} weitgehend ausoptimiert ist.

\begin{table}[htbp]
\centering
\caption{Laufzeit der Kernoperationen je Optimierungsstufe (in ns).}
\label{tab:optlevels}
\begin{tabular}{lrrr}
\toprule
\textbf{Operation} & \textbf{-O0} & \textbf{-O2} & \textbf{-O3} \\
\midrule
Addition $100\times100$        & 18\,835 & 3\,052 & 3\,864 \\
Subtraktion $100\times100$     & 21\,625 & 4\,039 & 3\,878 \\
Multiplikation $50\times50$    & 795\,848 & 60\,827 & 61\,417 \\
In-place \texttt{+=} $100\times100$ & 20\,170 & 3\,177 & 3\,897 \\
Verkettet $(A{+}B){-}(A{-}B)$  & 58\,763 & 11\,179 & 12\,130 \\
\bottomrule
\end{tabular}
\end{table}

\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.9\textwidth]{01_optimization_comparison.png}
  \caption{Laufzeitvergleich \texttt{-O2} gegenüber \texttt{-O3} für ausgewählte
  Operationen. Die höchste Optimierungsstufe bringt gegenüber \texttt{-O2}
  keinen durchgängigen Vorteil.}
  \label{fig:optlevels}
\end{figure}

Die Multiplikation dominiert das Laufzeitprofil deutlich: In einer gemischten
Arbeitslast aus Addition, Subtraktion, Multiplikation, in-place-Operation und
verketteten Ausdrücken entfällt der weitaus größte Anteil auf die
Multiplikation (Abbildung~\ref{fig:breakdown}). Dies entspricht der
Komplexität von $O(n^3)$ gegenüber $O(n^2)$ der übrigen Operationen.

\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.75\textwidth]{06_operation_breakdown.png}
  \caption{Anteil der Operationen an der Gesamtlaufzeit (\texttt{-O2}). Die
  Multiplikation überwiegt aufgrund ihrer kubischen Komplexität.}
  \label{fig:breakdown}
\end{figure}

Die Skalierung der Addition mit der Matrixgröße zeigt das erwartete
$O(n^2)$-Verhalten, allerdings überlagert von Cache-Effekten
(Tabelle~\ref{tab:skalierung}, Abbildung~\ref{fig:skalierung}). Beim Übergang von
$100\times100$ auf $500\times500$ wächst die Datenmenge um den Faktor~25, die
Laufzeit jedoch um etwa den Faktor~199. Eine $500\times500$-Matrix belegt rund
2\,MB und überschreitet damit den L2-Cache deutlich; die Operation wird
speicherbandbreitenbegrenzt. Der erweiterte Benchmark bestätigt diese Tendenz
bis $1000\times1000$ (rund 2{,}17\,ms bei \texttt{-O2}).

\begin{table}[htbp]
\centering
\caption{Laufzeit der Addition in Abhängigkeit von der Matrixgröße (\texttt{-O2}).}
\label{tab:skalierung}
\begin{tabular}{lrr}
\toprule
\textbf{Größe} & \textbf{Zeit} & \textbf{Faktor (ggü.\ $10\times10$)} \\
\midrule
$10\times10$   & 82\,ns    & 1 \\
$50\times50$   & 1\,135\,ns & 13{,}8 \\
$100\times100$ & 3\,052\,ns & 37{,}2 \\
$500\times500$ & 607\,987\,ns & 7\,414 \\
\bottomrule
\end{tabular}
\end{table}

\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.85\textwidth]{02_matrix_size_impact.png}
  \caption{Laufzeit der Addition über die Matrixgröße (\texttt{-O2}). Der
  überproportionale Anstieg bei $500\times500$ ist auf Cache-Effekte
  zurückzuführen.}
  \label{fig:skalierung}
\end{figure}

Den belastbarsten Hinweis auf den Nutzen des in-place-Entwurfs liefert der
direkte Vergleich von freier Funktion (\texttt{A + B}) und in-place-Operator
(\texttt{A += B}). Bei $100\times100$ ist die in-place-Variante etwa 25\,\%
schneller (3{,}46\,$\mu$s gegenüber 4{,}61\,$\mu$s), bei $500\times500$ rund das
2{,}6-Fache (235\,$\mu$s gegenüber 609\,$\mu$s). Der Unterschied entspricht
gerade jener einen Kopie des linken Operanden, die \texttt{operator+} intern
anlegt (Listing~\ref{lst:v3}) und die \texttt{operator+=} einspart. Bei sehr
kleinen Matrizen ($10\times10$) kehrt sich das Bild messtechnisch um; dieser
Effekt ist jedoch auf den in Abschnitt~\ref{sec:methodik} beschriebenen
Pausierungs-Aufwand zurückzuführen und nicht auf die Operation selbst.

\subsection{Ergebnisse: Temporäre Objekte}
\label{sec:tempobjekte}

Tabelle~\ref{tab:temporaries} weist die je Operation erzeugten Kopien und Moves
aus. Bemerkenswert ist, dass über alle Operationen hinweg kein einziger Move
gezählt wird. Dies ist kein Widerspruch zur vorhandenen Move-Semantik, sondern
eine unmittelbare Folge der Kopierauslassung: Die freie Funktion
\texttt{operator+} gibt eine benannte lokale Variable zurück (NRVO), und am
Aufrufort initialisiert ein prvalue das Ergebnisobjekt (garantierte Elision).
Beide Mechanismen entfernen den Rückgabe-Move vollständig. Die gezählten Kopien
stammen daher nicht aus der Rückgabe, sondern aus der einen Kopie des linken
Operanden innerhalb von \texttt{operator+} bzw.\ aus der bewussten Kopie zur
Vorbereitung der in-place-Messung.

\begin{table}[htbp]
\centering
\caption{Kopien und Moves je Operation (instrumentierte Zählung).}
\label{tab:temporaries}
\begin{tabular}{lrr}
\toprule
\textbf{Operation} & \textbf{Kopien / Op.} & \textbf{Moves / Op.} \\
\midrule
Addition ($A+B$)            & 1 & 0 \\
Subtraktion ($A-B$)         & 1 & 0 \\
Multiplikation ($A \cdot B$)& 0 & 0 \\
In-place ($\mathit{temp}=A;\ \mathit{temp}{+}{=}B$) & 1 & 0 \\
Verkettet ($A+B+C$)         & 2 & 0 \\
Gemischt                    & 2 & 0 \\
\bottomrule
\end{tabular}
\end{table}

Damit bestätigt sich die theoretische Vorhersage aus Abschnitt~2.6: Verkettete
Ausdrücke erzeugen proportional mehr Kopien (zwei statt einer), während die
Multiplikation -- die ihr Ergebnis intern aufbaut, ohne den linken Operanden zu
kopieren -- ganz ohne Kopie auskommt. Abbildung~\ref{fig:temporaries} stellt die
absoluten Zählwerte dar; da Google Benchmark je Operation unterschiedlich viele
Iterationen ausführt, sind für den Vergleich die Werte \emph{pro Operation} aus
Tabelle~\ref{tab:temporaries} maßgeblich.

\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.85\textwidth]{04_temporary_objects.png}
  \caption{Erzeugte temporäre Objekte (Kopien gegenüber Moves). Die
  Move-Operationen entfallen aufgrund der Kopierauslassung vollständig.}
  \label{fig:temporaries}
\end{figure}

\subsection{Ergebnisse: Speicherverbrauch}
\label{sec:speicher}

Das Speicherverhalten wird anhand der Kopier- und Move-Zähler in fünf
Szenarien untersucht (Tabelle~\ref{tab:heap}). Da die vereinheitlichte Klasse
Move-Semantik besitzt, gibt die Spalte ``gemessen'' die tatsächlich gezählten
Operationen wieder; die Spalte ``V1-Schätzung'' gibt analytisch an, wie viele
tiefe Kopien dieselben Szenarien ohne Move-Semantik verursachen würden (jede
Move-Operation wird dann zu einer Kopie).

\begin{table}[htbp]
\centering
\caption{Kopier- und Move-Operationen je Szenario (instrumentierte Zählung;
V1-Spalte als analytische Schätzung).}
\label{tab:heap}
\begin{tabular}{lrrr}
\toprule
\textbf{Szenario} & \textbf{Kopien} & \textbf{Moves} & \textbf{V1-Schätzung (Kopien)} \\
\midrule
Einfache Addition ($m_3 = m_1+m_2$)     & 1 & 1 & 2 \\
Verkettete Addition ($m_4 = m_1+m_2+m_3$) & 2 & 1 & 3 \\
Akkumulation (\texttt{+=}, 100$\times$)  & 0 & 0 & 100 \\
Vektor (100 Matrizen)                    & 100 & 127 & $>100$ \\
Temporäre Ausdrücke (1000$\times$)       & 2\,000 & 1\,000 & 3\,000 \\
\bottomrule
\end{tabular}
\end{table}

Die Akkumulationsschleife mit \texttt{operator+=} kommt ohne jede Kopie und
ohne jeden Move aus -- die Datenmengen werden ausschließlich in-place addiert.
Ohne zusammengesetzten Operator (\texttt{result = result + temp;}) wären je
Iteration eine Kopie und eine Zuweisung nötig. Beim Einfügen in einen
\texttt{std::vector} treten neben den 100 Kopien der Elemente zusätzliche
Move-Operationen bei den Reallokationen des Vektors auf; diese sind nur möglich,
weil der Move-Konstruktor \texttt{noexcept} ist (Abschnitt~2.4).
Abbildung~\ref{fig:heap} stellt die gemessenen Operationen der vereinheitlichten
Klasse der analytischen V1-Schätzung gegenüber.

\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.85\textwidth]{05_heap_usage_comparison.png}
  \caption{Kopier- und Move-Operationen je Szenario (V1 als analytische
  Schätzung gegenüber der move-fähigen Implementierung V2).}
  \label{fig:heap}
\end{figure}

\subsection{Interpretation der Ergebnisse}
\label{sec:interpretation}

Die Ergebnisse lassen sich zu vier belastbaren Aussagen verdichten.

\emph{Erstens} dominiert die algorithmische Komplexität die Laufzeit weit stärker
als der Entwurf der Operatoren. Die Multiplikation ($O(n^3)$) übersteigt
Addition und Subtraktion ($O(n^2)$) um mehr als eine Größenordnung und bestimmt
das Laufzeitprofil gemischter Arbeitslasten (Abbildung~\ref{fig:breakdown}).

\emph{Zweitens} liegt der mit Abstand größte Optimierungsgewinn im Übergang von
\texttt{-O0} zu \texttt{-O2}, während \texttt{-O3} keinen verlässlichen
Zusatznutzen bringt. Die Optimierungsstufe wirkt auf die Berechnungsschleifen,
nicht auf die Objektverwaltung; beide Achsen sind weitgehend unabhängig.

\emph{Drittens} zeigt sich der praktische Nutzen des in-place-Entwurfs am
deutlichsten dort, wo die Messung nicht durch den Pausierungs-Aufwand verfälscht
wird: Bei $500\times500$ ist \texttt{+=} rund 2{,}6-mal schneller als das freie
\texttt{+}, da letzteres eine vollständige Kopie des linken Operanden anlegt.
Dies ist der unmittelbar messbare Vorteil von Version~3 gegenüber Version~2.

\emph{Viertens} bestätigt die Zählung der temporären Objekte die theoretische
Erwartung: Unter C++17 entfernt die Kopierauslassung sämtliche Rückgabe-Moves,
weshalb sich naive und move-fähige Implementierungen bei der direkten
Initialisierung (\texttt{Matrix D = A + B;}) kaum unterscheiden. Der Nutzen der
Move-Semantik verlagert sich damit auf die Zuweisung aus temporären Objekten,
auf verkettete Ausdrücke und insbesondere auf Container-Reallokationen, wie
Tabelle~\ref{tab:heap} zeigt.

Einschränkend ist festzuhalten, dass die direkten Mikrobenchmarks von
Kopier- gegen Move-Konstruktor (gemessen 1\,523\,ns gegenüber 1\,175\,ns) und
von Kopier- gegen Move-Zuweisung (1\,382\,ns gegenüber 1\,552\,ns,
Abbildung~\ref{fig:copymove}) den theoretisch erwarteten großen Vorsprung der
Move-Operationen \emph{nicht} widerspiegeln. Ursache ist der in
Abschnitt~\ref{sec:methodik} beschriebene Pausierungs-Aufwand, der die wenige
Nanosekunden dauernde Zeigerübernahme überdeckt. Der eigentliche Beleg für den
Nutzen der Move-Semantik liegt daher nicht in diesen Einzelmessungen, sondern in
der Kombination aus den Operationszählungen (Tabelle~\ref{tab:temporaries},
\ref{tab:heap}) und dem skalierungsabhängigen Vorteil des in-place-Entwurfs.

\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.7\textwidth]{03_copy_vs_move.png}
  \caption{Kopier- gegenüber Move-Konstruktor ($100\times100$, \texttt{-O2}).
  Der geringe gemessene Abstand ist durch den Pausierungs-Aufwand des
  Messrahmens bedingt und unterschätzt den tatsächlichen Vorteil der
  Move-Operation.}
  \label{fig:copymove}
\end{figure}
"""

# Find line numbers for the section boundaries
start_line = None
end_line = None

for i, line in enumerate(lines):
    if r"\section{Experimente und Analyse}" in line:
        # Look back for the preceding comment line
        if i > 0 and "% " in lines[i-1] and "-" in lines[i-1]:
            start_line = i - 1
        else:
            start_line = i
    elif r"\section{Fazit}" in line:
        # Find the preceding comment
        if i > 0 and "% " in lines[i-1] and "-" in lines[i-1]:
            end_line = i - 1
        else:
            end_line = i

if start_line is not None and end_line is not None:
    # Replace the lines
    new_lines = lines[:start_line] + [new_sections + "\n"] + lines[end_line:]
    
    with open('seminararbeit.tex', 'w', encoding='utf-8') as f:
        f.writelines(new_lines)
    
    print("✓ Sections updated successfully!")
    print(f"  Replaced lines {start_line+1} through {end_line}")
    print(f"  New content added: ~{len(new_sections.splitlines())} lines")
else:
    print("✗ Error: Could not find section markers")
    if start_line is None:
        print("  Could not find 'Experimente und Analyse'")
    if end_line is None:
        print("  Could not find 'Fazit'")
