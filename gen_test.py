import xml.etree.ElementTree as ET
from xml.dom import minidom

def create_pnml(filename, net_id, places_data, trans_data, arcs_data):
    root = ET.Element("pnml")
    net = ET.SubElement(root, "net", id=net_id, type="http://www.pnml.org/version-2009/grammar/ptnet")
    page = ET.SubElement(net, "page", id="page1")

    for p_id, tokens in places_data:
        p = ET.SubElement(page, "place", id=p_id)
        name = ET.SubElement(p, "name")
        ET.SubElement(name, "text").text = p_id
        if tokens > 0:
            init = ET.SubElement(p, "initialMarking")
            ET.SubElement(init, "text").text = str(tokens)

    for t_id in trans_data:
        t = ET.SubElement(page, "transition", id=t_id)
        name = ET.SubElement(t, "name")
        ET.SubElement(name, "text").text = t_id

    for src, tgt in arcs_data:
        a_id = f"a_{src}_{tgt}"
        ET.SubElement(page, "arc", id=a_id, source=src, target=tgt)

    xmlstr = minidom.parseString(ET.tostring(root)).toprettyxml(indent="   ")
    with open(filename, "w") as f:
        f.write(xmlstr)
    print(f"Generated {filename}")

# --- TEST 1: 12 Parallel Processes (State space = 2^12 = 4096) ---
# Mô hình: Mỗi process có 2 trạng thái (Idle, Run). Transition Start và Stop.
places1 = []
trans1 = []
arcs1 = []
N_PROC = 12 

for i in range(N_PROC):
    pid_idle = f"P_idle_{i}"
    pid_run = f"P_run_{i}"
    tid_start = f"T_start_{i}"
    tid_stop = f"T_stop_{i}"

    places1.append((pid_idle, 1)) # Init token at Idle
    places1.append((pid_run, 0))
    
    trans1.extend([tid_start, tid_stop])

    # Idle -> Start -> Run
    arcs1.append((pid_idle, tid_start))
    arcs1.append((tid_start, pid_run))

    # Run -> Stop -> Idle
    arcs1.append((pid_run, tid_stop))
    arcs1.append((tid_stop, pid_idle))

create_pnml("test/complex_parallel.pnml", "parallel_12", places1, trans1, arcs1)

# --- TEST 2: 5 Dining Philosophers (Deadlock scenario) ---
# 5 Triết gia, 5 cái nĩa (Forks). Mỗi người cần 2 nĩa để ăn.
places2 = []
trans2 = []
arcs2 = []
N_PHILO = 5

for i in range(N_PHILO):
    # Places: Fork_i, Thinking_i, Eating_i
    places2.append((f"Fork_{i}", 1)) 
    places2.append((f"Think_{i}", 1))
    places2.append((f"Eat_{i}", 0))
    
    left = i
    right = (i + 1) % N_PHILO
    
    places2.append((f"HasL_{i}", 0)) # Đã cầm nĩa trái

    trans2.append(f"TakeL_{i}")
    trans2.append(f"TakeR_{i}")
    trans2.append(f"Drop_{i}")

    # 1. Think + ForkL -> TakeL -> HasL
    arcs2.append((f"Think_{i}", f"TakeL_{i}"))
    arcs2.append((f"Fork_{left}", f"TakeL_{i}"))
    arcs2.append((f"TakeL_{i}", f"HasL_{i}"))

    # 2. HasL + ForkR -> TakeR -> Eat
    arcs2.append((f"HasL_{i}", f"TakeR_{i}"))
    arcs2.append((f"Fork_{right}", f"TakeR_{i}"))
    arcs2.append((f"TakeR_{i}", f"Eat_{i}"))

    # 3. Eat -> Drop -> Think + ForkL + ForkR
    arcs2.append((f"Eat_{i}", f"Drop_{i}"))
    arcs2.append((f"Drop_{i}", f"Think_{i}"))
    arcs2.append((f"Drop_{i}", f"Fork_{left}"))
    arcs2.append((f"Drop_{i}", f"Fork_{right}"))

create_pnml("test/complex_deadlock.pnml", "dining_5", places2, trans2, arcs2)