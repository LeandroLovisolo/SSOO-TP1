((promedio=0))
for iter in {1..2000}
do
	((seed= $(rand) * $iter))
	temp=$(./simusched ejercicio9/tareas.tsk 1 0 0 SchedLottery 10 $seed | grep EXIT | awk '{ print $2 }')
	ultima=$(echo $temp | awk '{print $4}')
	primera=$(echo $temp | awk '{print $1}')
	((promedio= ($promedio*($iter-1) + $ultima - $primera)/$iter ))
	echo $iter,$promedio
done
